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
//<ID> 599
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
    cJSON *names = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateString("alice");
    cJSON_AddItemToArray(names, n1);
    cJSON *n2 = cJSON_CreateString("bob");
    cJSON_AddItemToArray(names, n2);
    cJSON *n3 = cJSON_CreateString("carol");
    cJSON_AddItemToArray(names, n3);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    int size_original = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)size_original);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "owner", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_names = cJSON_HasObjectItem(parsed, "names");
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first_name_item = cJSON_GetArrayItem(parsed_names, 0);
    char *name = cJSON_GetStringValue(first_name_item);
    cJSON_bool is_invalid = cJSON_IsInvalid(first_name_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has=%d;invalid=%d;name=%s",
            version ? version : "null",
            (int)has_names,
            (int)is_invalid,
            name ? name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}