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
//<ID> 1041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"alpha\":10}";
    const char json2[] = "{\"existing\":1,\"list\":[1,2]}";
    const char *parse_end = NULL;
    cJSON *root1 = cJSON_Parse(json1);
    cJSON *root2 = cJSON_ParseWithLengthOpts(json2, (size_t)(sizeof(json2) - 1), &parse_end, 1);
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(3.0);
    cJSON *num_b = cJSON_CreateNumber(4.5);
    cJSON *sref = cJSON_CreateStringReference("steady");
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root2, "added", arr);
    cJSON_AddItemToObject(root2, "extra", cJSON_CreateNumber(7.25));

    // step 2: Configure
    cJSON *existing_item = cJSON_GetObjectItem(root2, "existing");
    double existing_val = cJSON_GetNumberValue(existing_item);
    cJSON *derived = cJSON_CreateNumber(existing_val + 2.0);
    cJSON_AddItemToObject(root2, "derived", derived);

    // step 3: Operate & Validate
    char *out_root2 = cJSON_PrintUnformatted(root2);
    char *out_root1 = cJSON_Print(root1);

    // step 4: Cleanup
    cJSON_free(out_root2);
    cJSON_free(out_root1);
    cJSON_Delete(root2);
    cJSON_Delete(root1);
    return 66; // API sequence test completed successfully
}