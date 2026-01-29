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
//<ID> 1031
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darray = NULL;
    cJSON *flag = NULL;
    cJSON *got_measurements = NULL;
    cJSON *got_flag = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    cJSON *sum_item = NULL;
    char *printed = NULL;
    const double nums[3] = {10.5, 20.25, 30.75};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "measurements", darray);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate / Validate
    got_measurements = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "active");
    first = cJSON_GetArrayItem(got_measurements, 0);
    second = cJSON_GetArrayItem(got_measurements, 1);
    double sum_first_two = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    sum_item = cJSON_CreateNumber(sum_first_two);
    cJSON_AddItemToObject(root, "sum_first_two", sum_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}