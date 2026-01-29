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
//<ID> 1024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(data, n0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "label", cJSON_CreateString("sample"));
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate & Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "count_double", count_val * 2.0);
    cJSON *n_mid = cJSON_CreateNumber(15.0);
    cJSON_InsertItemInArray(data, 1, n_mid);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddNumberToObject(root, "meta_flag", (double)meta_is_obj);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}