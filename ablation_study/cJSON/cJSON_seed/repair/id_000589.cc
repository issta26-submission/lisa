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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_InsertItemInArray(numbers, 0, n0);
    cJSON_InsertItemInArray(numbers, 1, n1);
    cJSON_InsertItemInArray(numbers, 2, n2);

    // step 2: Configure
    cJSON *got_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(got_meta, "version");
    double version_value = cJSON_GetNumberValue(version_item);
    cJSON_InsertItemInArray(numbers, 1, cJSON_CreateNumber(25.0));
    cJSON_AddNumberToObject(root, "meta_version_times_two", version_value * 2.0);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(got_numbers, 0);
    cJSON *a1 = cJSON_GetArrayItem(got_numbers, 1);
    cJSON *a2 = cJSON_GetArrayItem(got_numbers, 2);
    cJSON *a3 = cJSON_GetArrayItem(got_numbers, 3);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2) + cJSON_GetNumberValue(a3);
    double count = 4.0 + (double)(!(!has_numbers) - 0); // uses has_numbers to keep data flow; results in 4.0
    double average = sum / count;
    cJSON_AddNumberToObject(root, "average", average);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}