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
//<ID> 412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"array\":[1,2,3],\"obj\":{\"a\":1}}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 0);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "array");
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *newobj = cJSON_CreateObject();
    cJSON *scaled_num = cJSON_CreateNumber(second_val * 10.0);
    cJSON_AddItemToObject(newobj, "scaled", scaled_num);

    // step 3: Operate
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, newobj);
    cJSON_AddBoolToObject(root, "insert_ok", insert_ok);
    cJSON *inserted = cJSON_GetArrayItem(arr, 1);
    cJSON *extracted_scaled = cJSON_GetObjectItemCaseSensitive(inserted, "scaled");
    double extracted_val = cJSON_GetNumberValue(extracted_scaled);
    cJSON_AddNumberToObject(root, "inserted_scaled", extracted_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}