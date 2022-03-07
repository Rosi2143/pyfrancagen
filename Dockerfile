FROM rb-dtr.de.bosch.com/osd/osd6
LABEL maintainer="Michael Rossner <Michael.Rossner@de.bosch.com>"

# Install packages for building the architecture repo
RUN apt-get -qq update && \
    apt-get --no-install-recommends -y install apt-utils unzip wget curl vim python3 python3-pip make cmake clang-format

# Fix certificate issues, found as of
# https://bugs.launchpad.net/ubuntu/+source/ca-certificates-java/+bug/983302
RUN apt-get update && \
	apt-get install -y ca-certificates-java && \
	apt-get clean && \
	update-ca-certificates -f && \
	rm -rf /var/lib/apt/lists/* && \
	rm -rf /var/cache/oracle-jdk8-installer;

RUN apt-get autoremove \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install --no-cache-dir -U pip
RUN python3 -m pip install --no-cache-dir setuptools
RUN python3 -m pip install --no-cache-dir markupsafe==2.0.1
RUN python3 -m pip install --no-cache-dir ply

# install pyfranca
# https://github.com/gunnarx/pyfranca/tree/master#installation
# RUN python3 -m pip install --no-cache-dir pyfranca --> wrong version

ADD ./pyfranca/ pyfranca/
RUN \
    cd pyfranca && \
	 python3 setup.py install

RUN apt-get update && \
	apt-get install -y doxygen

ENV HOME /home/ubuntu