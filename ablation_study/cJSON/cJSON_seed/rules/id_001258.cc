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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"values\":[1.5,2.5,3.5]}";
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *first = NULL;
    double v0 = 0.0;
    double nums[3];
    cJSON *computed_arr = NULL;
    cJSON *retrieved_item = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    values = cJSON_GetObjectItem(root, "values");
    first = cJSON_GetArrayItem(values, 0);

    // step 3: Operate / Validate
    v0 = cJSON_GetNumberValue(first);
    nums[0] = v0;
    nums[1] = v0 + 10.0;
    nums[2] = 42.0;
    computed_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "computed", computed_arr);
    cJSON_AddStringToObject(root, "status", "processed");
    cJSON_AddRawToObject(root, "raw", "{\"rawKey\":123}");
    cJSON_AddNullToObject(root, "nothing");
    retrieved_item = cJSON_GetArrayItem(computed_arr, 1);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}