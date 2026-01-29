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
//<ID> 639
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
    cJSON *entry = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("alpha");

    // step 2: Configure
    cJSON_AddItemToObject(entry, "num", num_item);
    cJSON_AddItemToObject(entry, "name", str_item);
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON_AddItemToObject(root, "list", list);
    cJSON *dup_entry = cJSON_Duplicate(entry, 1);
    cJSON_AddItemToArray(list, dup_entry);
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(list, extra_num);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);
    sprintf((char *)buffer, "%s", snapshot);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_entry = cJSON_GetObjectItem(parsed, "entry");
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    int list_count = cJSON_GetArraySize(parsed_list);
    cJSON *parsed_entry_num = cJSON_GetObjectItem(parsed_entry, "num");
    double parsed_number = cJSON_GetNumberValue(parsed_entry_num);
    (void)version;
    (void)parsed_number;
    cJSON_DeleteItemFromObject(parsed, "entry");
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}