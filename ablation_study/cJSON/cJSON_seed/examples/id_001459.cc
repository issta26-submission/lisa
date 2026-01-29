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
//<ID> 1459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array1 = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("example");
    cJSON *bool1 = cJSON_CreateBool(1);
    cJSON *str2 = cJSON_CreateString("example");
    cJSON *bool2 = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(array1, str1);
    cJSON_AddItemToArray(array1, bool1);
    cJSON_AddItemToArray(array2, str2);
    cJSON_AddItemToArray(array2, bool2);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(array1);
    cJSON_bool equal = cJSON_Compare(array1, array2, 1);
    (void)equal;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(array1);
    cJSON_Delete(array2);

    // API sequence test completed successfully
    return 66;
}