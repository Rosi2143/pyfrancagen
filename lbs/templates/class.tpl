// Generated from Franca IDL Interface {{ fqn }}
// using https://jinja.palletsprojects.com/en/2.10.x/
// {{ timestamp }}

{{ boilerplate }}

{%- for inc in includes %}
#include "{{inc}}.types.h"
{%- endfor %}
#include "{{name}}.h"

// Constructors, etc.
{{ name }}::{{ name }}() { /* TODO, or use implicit */ }

{{ name }}::~{{ name}} () { /* TODO, or use implicit */ }

{% for m in item.methods.values() %}
// functions for {{ m.name }}
// Commands
{% set maybecomma = joiner(",") %}
void {{ name }}::{{m.name}}_command_serialize (
                           const UInt16& act{{ maybecomma() }}
                           {%- for p in m.in_args.values() -%}
                              {{ maybecomma() }}const {{ render_type(p) }}& {{ p.name }}{% endfor %}
                           , buffer &message_buffer
                       )
{
   /* Method body for {{ m.name }} here */
  UInt8 fid = (UInt8)FID::FID_{{ m.name|upper }};
  serializer container;
  container.push_back(fid);
  container.push_back(act);

   {%- for p in m.in_args.values() -%}
      container.push_back({{ p.name }});
   {% endfor %}

   message_buffer = container.dump_container();
};

{% set maybecomma = joiner(",") %}
bool {{ name }}::{{m.name}}_command_deserialize (
                           const buffer &message_buffer,
                           UInt16& act{{ maybecomma() }}
                           {%- for p in m.in_args.values() -%}
                              {{ maybecomma() }} {{ render_type(p) }}& {{ p.name }}{% endfor %}
                       )
{
   /* Method body for {{ m.name }} here */
  UInt8 fid = 0;
  serializer container(message_buffer);
  fid = container.pop_front(fid);
  if (fid != (UInt8)FID::FID_{{ m.name|upper }}){
     return false;
  }
  act = container.pop_front(act);

  {%- for p in m.in_args.values() -%}
     {{ p.name }} = container.pop_front({{ p.name }});
  {% endfor %}

  return true;
};

// Responses
{% set maybecomma = joiner(",") %}
void {{ name }}::{{m.name}}_response_serialize (
                           const UInt16 &act{{ maybecomma() }}
                           {%- for p in m.out_args.values() -%}
                              {{ maybecomma() }} const {{ render_type(p)}} &{{ p.name }}{% endfor %}
                           , buffer &message_buffer
                       )
{
   /* Method body for {{ m.name }} here */
  UInt8 fid = (UInt8)FID::FID_{{ m.name|upper }} + 1;
  serializer container;
  container.push_back(fid);
  container.push_back(act);

   {%- for p in m.out_args.values() -%}
      container.push_back({{ p.name }});
   {% endfor %}

   message_buffer = container.dump_container();
};

{% set maybecomma = joiner(",") %}
bool {{ name }}::{{m.name}}_response_deserialize (
                           const buffer &message_buffer,
                           UInt16& act{{ maybecomma() }}
                           {%- for p in m.out_args.values() -%}
                              {{ maybecomma() }} {{ render_type(p) }}& {{ p.name }}{% endfor %}
                       )
{
   /* Method body for {{ m.name }} here */
  UInt8 fid = 0;
  serializer container(message_buffer);
  fid = container.pop_front(fid);
  if (fid != (UInt8)(FID::FID_{{ m.name|upper }}) + 1){
     return false;
  }
  act = container.pop_front(act);

  {%- for p in m.out_args.values() -%}
     {{ p.name }} = container.pop_front({{ p.name }});
  {% endfor %}

  return true;
};
{% endfor %}