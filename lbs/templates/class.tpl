// Generated from Franca IDL Interface {{ fqn }}
// using https://jinja.palletsprojects.com/en/2.10.x/
// {{ timestamp }}

{{ boilerplate }}

{%- for inc in includes %}
#include "{{inc}}.types.h"
{%- endfor %}
#include "{{name}}.h"

const uint8_t len_index = 3;

// Constructors, etc.
{{ name }}::{{ name }}() { /* TODO, or use implicit */ }

{{ name }}::~{{ name}} () { /* TODO, or use implicit */ }

// #################
// functions methods
// #################
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
  UInt16 messageLength = 0;
  serializer container;
  container.push_back(fid);
  container.push_back(act);

  container.push_back(messageLength);

  {%- for p in m.in_args.values() -%}
  messageLength += container.push_back({{ p.name }});
  {% endfor %}

  message_buffer = container.dump_container();

  message_buffer[len_index] = (messageLength & 0xFF);
  message_buffer[len_index + 1] = ((messageLength >> 8) & 0xFF);
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
  UInt16 parameterLength = 0;
  serializer container(message_buffer);
  fid = container.pop_front(fid, parameterLength);
  if (fid != (UInt8)FID::FID_{{ m.name|upper }}){
     return false;
  }
  act = container.pop_front(act, parameterLength);

  UInt16 messageLength_expected = 0;
  UInt16 messageLength_received = container.pop_front(messageLength_expected, parameterLength);

  {%- for p in m.in_args.values() -%}
  {{ p.name }} = container.pop_front({{ p.name }}, parameterLength);
  messageLength_expected += parameterLength;
  {% endfor %};

  if (messageLength_expected != messageLength_received) {
    return false;
  }

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
  UInt16 messageLength = 0;
  serializer container;
  container.push_back(fid);
  container.push_back(act);

  container.push_back(messageLength);

  {%- for p in m.out_args.values() -%}
  messageLength += container.push_back({{ p.name }});
  {% endfor %}

  message_buffer = container.dump_container();

  message_buffer[len_index] = (messageLength & 0xFF);
  message_buffer[len_index + 1] = ((messageLength >> 8) & 0xFF);
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
  UInt16 parameterLength = 0;
  serializer container(message_buffer);
  fid = container.pop_front(fid, parameterLength);
  if (fid != (UInt8)(FID::FID_{{ m.name|upper }}) + 1){
     return false;
  }
  act = container.pop_front(act, parameterLength);

  UInt16 messageLength_expected = 0;
  UInt16 messageLength_received = container.pop_front(messageLength_received, parameterLength);

  {%- for p in m.out_args.values() -%}
  {{ p.name }} = container.pop_front({{ p.name }}, parameterLength);
  messageLength_expected += parameterLength;
  {% endfor %};

  if (messageLength_expected != messageLength_received) {
    return false;
  }

  return true;
};
{% endfor %}

// ####################
// functions broadcasts
// ####################
{% for m in item.broadcasts.values() %}
// functions for {{ m.name }}
{% set maybecomma = joiner(",") %}
void {{ name }}::{{ m.name }}_broadcast_serialize (
                  const UInt16& act{{ maybecomma() }}
                  {%- for p in m.out_args.values() -%}
                     {{ maybecomma() }}const {{ render_type(p) }}& {{ p.name }}{% endfor %}
                  , buffer &message_buffer
                  ){
  /* Method body for {{ m.name }} here */
  UInt8 fid = (UInt8)FID::FID_{{ m.name|upper }} + 1;
  UInt16 messageLength = 0;
  serializer container;
  container.push_back(fid);
  container.push_back(act);

  container.push_back(messageLength);

  {%- for p in m.out_args.values() -%}
  messageLength += container.push_back({{ p.name }});
  {% endfor %}

  message_buffer = container.dump_container();

  message_buffer[len_index] = (messageLength & 0xFF);
  message_buffer[len_index + 1] = ((messageLength >> 8) & 0xFF);
}

{% set maybecomma = joiner(",") %}
bool {{ name }}::{{ m.name }}_broadcast_deserialize (const buffer &message_buffer,
                  UInt16& act{{ maybecomma() }}
                  {%- for p in m.out_args.values() -%}
                     {{ maybecomma() }} {{ render_type(p) }}& {{ p.name }}{% endfor %}
                  ){
  /* Method body for {{ m.name }} here */
  UInt8 fid = 0;
  UInt16 parameterLength = 0;
  serializer container(message_buffer);
  fid = container.pop_front(fid, parameterLength);
  if (fid != (UInt8)(FID::FID_{{ m.name|upper }}) + 1){
     return false;
  }
  act = container.pop_front(act, parameterLength);

  UInt16 messageLength_expected = 0;
  UInt16 messageLength_received = container.pop_front(messageLength_received, parameterLength);

  {%- for p in m.out_args.values() -%}
  {{ p.name }} = container.pop_front({{ p.name }}, parameterLength);
  messageLength_expected += parameterLength;
  {% endfor %};

  if (messageLength_expected != messageLength_received) {
    return false;
  }

  return true;
}
{%endfor%}
