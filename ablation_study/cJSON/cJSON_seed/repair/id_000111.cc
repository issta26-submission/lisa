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
//<ID> 111
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_backup", dup_numbers);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_item = cJSON_GetArrayItem(got_numbers, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *scale_item = cJSON_GetObjectItem(got_meta, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    double scaled = first_val * scale_val;
    cJSON_AddNumberToObject(root, "first_scaled", scaled);
    int backup_size = cJSON_GetArraySize(dup_numbers);
    cJSON_AddNumberToObject(root, "backup_size", (double)backup_size);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}