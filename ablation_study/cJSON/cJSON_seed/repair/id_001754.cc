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
//<ID> 1754
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(20));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.0"));
    cJSON_AddItemToObject(meta, "active", cJSON_CreateTrue());
    char *printed = cJSON_Print(root);

    // step 3: Operate and Validate
    cJSON *parsed = cJSON_Parse(printed);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    char *printed_parsed = cJSON_Print(parsed);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = printed_parsed ? printed_parsed[0] : 'Y';
    buffer[2] = (char)('0' + (int)equal);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_parsed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}