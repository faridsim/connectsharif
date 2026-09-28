//for one translation unit, #pragma once means the header will be included at most once.
#pragma once
//using enum class instead of enum to prevent clashing
//?can be used for unit-8

enum class MessageType
{
    Email,
    Sms,
    Push
};
