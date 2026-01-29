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
//<ID> 1191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(values, s1);
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(values, s2);
    cJSON *s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(values, s3);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "original list");

    // step 3: Operate
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_Minify(flat);
    cJSON *parsed = cJSON_Parse(flat);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON_DeleteItemFromArray(parsed_values, 1);

    // step 4: Validate & Cleanup
    int remaining = cJSON_GetArraySize(parsed_values);
    (void)remaining;
    cJSON_free(flat);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}