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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(list, num0);
    cJSON_AddItemToArray(list, maybe_null);
    cJSON_AddItemToArray(list, num1);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddStringToObject(root, "creator", "tester");

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    int parsed_count = cJSON_GetArraySize(parsed_list);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(list, maybe_null, replacement);
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)version;
    (void)parsed_count;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}