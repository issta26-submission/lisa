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
//<ID> 1184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    double doubles[] = {1.1, 2.2, 3.3};
    float floats[] = {4.4f, 5.5f};
    cJSON *darr = cJSON_CreateDoubleArray(doubles, 3);
    cJSON *farr = cJSON_CreateFloatArray(floats, 2);
    cJSON_AddItemToArray(array, darr);
    cJSON_AddItemToArray(array, farr);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON_AddNumberToObject(root, "scale", 2.0);
    cJSON_AddNumberToObject(root, "offset", -1.5);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *scale_item = cJSON_GetObjectItem(root, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON *numbers_item = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_array = cJSON_GetArrayItem(numbers_item, 0); // darr
    cJSON *first_value = cJSON_GetArrayItem(first_array, 0);
    double v0 = cJSON_GetNumberValue(first_value);
    double computed = v0 * scale + cJSON_GetNumberValue(cJSON_GetObjectItem(root, "offset"));
    cJSON_AddNumberToObject(root, "computed", computed);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool same = cJSON_Compare(root, dup, 1);
    cJSON_AddNumberToObject(root, "same", (double)same);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}