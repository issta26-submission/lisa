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
//<ID> 1867
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *dup_a = NULL;
    cJSON *arr_item = NULL;
    char *out = NULL;
    double extracted = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num_a = cJSON_AddNumberToObject(root, "a", 3.14);
    num_b = cJSON_AddNumberToObject(root, "b", 2.71);

    // step 3: Core operations
    num_a = cJSON_GetObjectItem(root, "a");
    dup_a = cJSON_Duplicate(num_a, 1);
    cJSON_AddItemToArray(arr, dup_a);
    arr_item = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetNumberValue(arr_item);

    // step 4: Validate
    out = cJSON_PrintUnformatted(root);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(dup_a != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(extracted == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}