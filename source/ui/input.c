#include "input.h"

kb_key_t _key_group_1;
kb_key_t _key_group_2;
kb_key_t _key_group_6;
kb_key_t _key_group_7;

void input_scan(void)
{
    kb_Scan();
    _key_group_1 = kb_Data[kb_group_1];
    _key_group_2 = kb_Data[kb_group_2];
    _key_group_6 = kb_Data[kb_group_6];
    _key_group_7 = kb_Data[kb_group_7];
}
