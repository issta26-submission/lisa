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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *count_item = NULL;
    cJSON *retrieved_count = NULL;
    char *printed = NULL;
    double count_val = 0.0;
    double adjusted_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    count_item = cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(retrieved_count);
    adjusted_val = count_val + 1.0;
    cJSON_AddNumberToObject(root, "adjusted_count", adjusted_val);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}