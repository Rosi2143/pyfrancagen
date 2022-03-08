#pragma once
// Generated from Franca IDL Interface {{ fqn }}
// {{ timestamp }}

{{ boilerplate }}

#include "serializer.h"
#include "{{name}}.types.h"

{%- for i in imports %}
#include "{{ i.name }}.types.h"
{%- endfor %}

class {{ name }}
{
    public:
        {{ name }} ();
        ~{{ name}} ();

    // get the function ID for any message_buffer
    UInt8 getFid(const buffer &message_buffer) const {return message_buffer[fid_index];}
    {% for m in item.methods.values() %}
    // functions for {{ m.name }}
    // Commands
    {% set maybecomma = joiner(",") %}
    void {{ m.name }}_command_serialize (
                        const UInt16& act{{ maybecomma() }}
                        {%- for p in m.in_args.values() -%}
                          {{ maybecomma() }}const {{ render_type(p) }}& {{ p.name }}{% endfor %}
                        , buffer &message_buffer
                      );
    {% set maybecomma = joiner(",") %}
    bool {{ m.name }}_command_deserialize (
                        const buffer &message_buffer,
                        UInt16& act{{ maybecomma() }}
                        {%- for p in m.in_args.values() -%}
                          {{ maybecomma() }} {{ render_type(p) }}& {{ p.name }}{% endfor %}
                      );

    // Responses
    {% set maybecomma = joiner(",") %}
    void {{ m.name }}_response_serialize (
                           const UInt16 &act{{ maybecomma() }}
                           {%- for p in m.out_args.values() -%}
                              {{ maybecomma() }} const {{ render_type(p)}} &{{ p.name }}{% endfor %}
                           , buffer &message_buffer
                      );
    {% set maybecomma = joiner(",") %}
    bool {{ m.name }}_response_deserialize (
                           const buffer &message_buffer,
                           UInt16& act{{ maybecomma() }}
                           {%- for p in m.out_args.values() -%}
                              {{ maybecomma() }} {{ render_type(p) }}& {{ p.name }}{% endfor %}
                      );

    {%endfor%}

  private:
    const uint8_t fid_index = 0;
};
