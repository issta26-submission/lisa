#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *flag_extra = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddItemToArray(flags, flag_false);
    cJSON_AddItemToArray(flags, flag_extra);
    cJSON_AddItemToObjectCS(root, "flags", flags);
    cJSON_AddItemToObjectCS(root, "enabled", cJSON_CreateBool(1));

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *printed_ok = cJSON_CreateBool(printed);
    cJSON *printed_text = cJSON_CreateString(buffer);
    cJSON_AddItemToObjectCS(root, "printed_ok", printed_ok);
    cJSON_AddItemToObjectCS(root, "printed_text", printed_text);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}