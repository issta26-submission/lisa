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
//<ID> 944
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
    cJSON_AddNumberToObject(root, "existing", 100.0);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_InsertItemInArray(numbers, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(2.75);
    cJSON_InsertItemInArray(numbers, 1, n1);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "second_value", second_val);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    cJSON_PrintPreallocated(root, (char *)buf, 512, 1);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}