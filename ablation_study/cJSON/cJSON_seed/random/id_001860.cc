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
//<ID> 1860
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
    cJSON *retrieved_one = NULL;
    cJSON *dup_item = NULL;
    cJSON *arr_item = NULL;
    char *out = NULL;
    double val_two = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Core operations
    cJSON_AddNumberToObject(root, "one", 1.5);
    cJSON_AddNumberToObject(root, "two", 2.5);
    retrieved_one = cJSON_GetObjectItem(root, "one");
    dup_item = cJSON_Duplicate(retrieved_one, 0);
    cJSON_AddItemToArray(arr, dup_item);
    arr_item = cJSON_GetArrayItem(arr, 0);
    val_two = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "two"));
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(retrieved_one != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_two == 2.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}