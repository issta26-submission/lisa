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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"obj\":{\"key\":\"value\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *obj = cJSON_GetObjectItemCaseSensitive(root, "obj");

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *new_object = cJSON_CreateObject();
    cJSON *scaled_number = cJSON_CreateNumber(first_val * 10.0);
    cJSON_AddItemToObject(new_object, "scaled", scaled_number);
    cJSON *note_str = cJSON_CreateString("inserted");
    cJSON_AddItemToObject(new_object, "note", note_str);

    // step 3: Operate
    cJSON_InsertItemInArray(arr, 1, new_object);

    // step 4: Validate and Cleanup
    cJSON *at_index_1 = cJSON_GetArrayItem(arr, 1);
    cJSON *note_found = cJSON_GetObjectItemCaseSensitive(at_index_1, "note");
    const char *note_text = cJSON_GetStringValue(note_found);
    (void)note_text;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}