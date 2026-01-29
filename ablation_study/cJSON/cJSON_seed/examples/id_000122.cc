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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *created_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, created_num);
    cJSON *direct_item = cJSON_AddNumberToObject(root, "direct", 3.1415);

    // step 2: Configure
    cJSON *retrieved_direct = cJSON_GetObjectItem(root, "direct");
    double direct_value = 0.0;
    direct_value = cJSON_GetNumberValue(retrieved_direct);
    cJSON *extra_num = cJSON_CreateNumber(direct_value + 10.0);
    cJSON_AddItemToArray(arr, extra_num);
    cJSON_AddNumberToObject(root, "direct_plus_ten", direct_value + 10.0);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    (void)printed; // printed is used meaningfully below when freed

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}