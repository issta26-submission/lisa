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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    const char *json_text = "{\"id\":42,\"name\":\"example\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *note = cJSON_CreateString("note");

    // step 2: Configure
    cJSON_AddItemToArray(arr, parsed);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToArray(arr, note);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *id_item = cJSON_GetObjectItem(first, "id");
    double first_id = cJSON_GetNumberValue(id_item);
    cJSON_AddNumberToObject(root, "first_id", first_id);
    const char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(first, "name"));
    cJSON_AddStringToObject(root, "first_name", first_name);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}