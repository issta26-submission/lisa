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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_str = cJSON_Version();
    cJSON *root = cJSON_Parse("[]");

    // step 2: Configure
    cJSON *first_num = cJSON_CreateNumber((double)(unsigned char)version_str[0]);
    cJSON_AddItemToArray(root, first_num);
    cJSON *second_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(root, second_num);

    // step 3: Operate & Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 1));
    cJSON *sum_num = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToArray(root, sum_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}