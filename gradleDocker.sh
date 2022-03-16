set -e

parameters=$@
docker_osd_repo_server=rb-dtr.de.bosch.com/osd
docker_daicivic_repo_server=daicivic-docker-repos.rb-cmbinex-hi-p1.de.bosch.com/ssa/
docker_image=
version=1.0
dry_run=false
verbose=false
buildCommand="python3 ./generate_icc.py"
do_check=false
do_build=false
mappedWorkDirectory="/home/ubuntu/work"
uid=`id -u`
gid=`id -g`
timezone=`cat /etc/timezone`

# helper function to remove a docker if not rm by docker parameter
function removeDockerContainer () {
   if [ -z $rm ]; then
     # Do the removal of container, we don't use -rm in docker run due to NAVARCH-1570
     # (removal of rootfs is failing)
     echo "Removing docker container ${container}"
     # With || true we ignore failure of the docker rm command
     # (will be removed by the cleanup of docker containers on jenkins anyway)
     docker rm -f ${container} || true
   fi
}

if [ "$BUILD_DOCKER__USE_LOCAL_IMAGES" != "" ]; then
   docker_osd_repo_server=""
   version=latest
fi

while [ $# -gt 0 ]
do
   case $1 in
      --image=*) docker_image=${1:8};;
      --dry-run) dry_run=true;;
      --verbose) verbose=true;;
      -v) verbose=true;;
      --run=*) buildCommand=${1:6};;
      --check) do_check=true;;
      --build) do_build=true;;
      --version=*) version=${1:9};;

      *) " "$1;;
   esac
   shift
done


container=build_${BUILD_TAG//[,=]/_}
docker_image_url=${docker_daicivic_repo_server}${docker_image}:$version

# Check that we dont run on Jenkins
if [ -z $JENKINS_URL ]; then
   # -i is required to forward Ctrl+C to the process in the container
   # but jenkins doesn't support it
   interactive=-i
   rm=--rm
fi
# TODO implement handling for Jenkins, e.g. removing the docker container

command="docker run ${name} ${rm} --user ${uid}:${gid} --privileged ${interactive} ${tty} \
   $cache_options
   -v ${PWD}:${mappedWorkDirectory} \
   ${products_volume}\
   ${licenseFileMapping}\
   ${dockerEnvironment} \
   -e DOCKER_IMAGE=${docker_image} \
   -e DOCKER_IMAGE_VERSION=${version} \
   -e DOCKER_HOST=$HOSTNAME \
   -e DOCKER_HOST_USER=`basename ${HOME}` \
   -e USER=${USERNAME} \
   -e LANG=${LANG} \
   -e LANGUAGE=${LANGUAGE} \
   -e TZ=${timezone} \
   -e BUILD_NUMBER=${BUILD_NUMBER} \
   -e JOB_URL=${JOB_URL} \
   -e REPO_LOCATION=`git config remote.origin.url` \
   -e DAI6HI_COMMON_CREDS_USR=${DAI6HI_COMMON_CREDS_USR} \
   -e DAI6HI_COMMON_CREDS_PSW=${DAI6HI_COMMON_CREDS_PSW} \
   -e API_KEY=${API_KEY} \
   ${sshDockerOptions} \
   ${docker_image_url} \
   /bin/bash -l -c \"cd ${mappedWorkDirectory}/icc && $buildCommand $thinned_out_parameters\""
#echo ${timezone} > /etc/timezone &&

if [ $verbose == true ]; then
   echo "command line parameters: " $parameters
   echo "detected toolchain:      " $toolchain
   echo "docker image:            " $docker_image
   echo "build parameters:        " $thinned_out_parameters
   echo "sshDockerOptions:        " $sshDockerOptions
   echo "dockerEnvironment:       " $dockerEnvironment
   echo "buildCommand:            " $buildCommand
   echo "in-source:               " $in_source
   echo "dry run:                 " $dry_run
   echo "user:                    " $uid
   echo "group:                   " $gid
   echo "timezone:                " $timezone
   echo "command:                 " $command
fi

if [ do_check <> true ]; then
   echo "Running in docker image ${docker_image_url}"
fi

if [ $do_check == true ]; then
   set +e
   docker_inspect_command="docker image inspect ${docker_image_url}"
   if [ $verbose == true ]; then
      echo "${docker_inspect_command}"
   fi
   ${docker_inspect_command} &>/dev/null 2>&1
   dockerInspectExitCode=$?

   if [ $verbose == true ]; then
      echo "do_check: Exiting with ${dockerInspectExitCode}"
   fi
   if [ ${dockerInspectExitCode} != 0 ]; then
      echo "run below command to download/create container"
      echo -e "\n\t->  $0  --image=\"${docker_image}\" --build\n"
   fi
   exit ${dockerInspectExitCode}
fi

if [ $do_build == true ]; then
   set +e
   image_check_command="$0 --image=${docker_image} --check"
   dockerBuildExitCode=0
   ${image_check_command}

   if [ $? == 0 ]; then
      echo "Image ${docker_image_url} already exists"
   else
      echo "Image ${docker_image_url} does not exists --> check docker registry"
      docker login daicivic-docker-repos.rb-cmbinex-hi-p1.de.bosch.com
      registry_download_command="docker pull ${docker_image_url}"
      ${registry_download_command}
      if [ $? == 0 ]; then
         echo "Image ${docker_image_url} downloaded from Artifactory docker registry"
      else
         echo "Image ${docker_image_url} does not exists in docker registry --> build is locally"
         docker_build_command="docker build --tag ${docker_image_url} ."
         ${docker_build_command}
         dockerBuildExitCode=$?
      fi
   fi

   if [ $verbose == true ]; then
      echo "do_build: Exiting with ${dockerBuildExitCode}"
   fi
   exit ${dockerBuildExitCode}
fi

if [ $dry_run == false ]; then

   set +e
   image_check_command="$0 --image=${docker_image} --check"
   ${image_check_command}
   if [ $? != 0 ]; then
      echo "exiting as no valid image is available"
      exit $?
   fi

   eval ${command}
   dockerRunExitCode=$?

   set -e
   removeDockerContainer

   echo "run: Exiting with ${dockerRunExitCode}"
   exit ${dockerRunExitCode}
else
   echo ${command}
fi
