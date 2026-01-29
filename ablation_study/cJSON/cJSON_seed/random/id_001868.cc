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
//<ID> 1868
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
    cJSON *num_direct = NULL;
    cJSON *dup_item = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *obj_first = NULL;
    char *out = NULL;
    double val0 = 0.0;
    double val_first = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddNumberToObject(root, "first", 1.23);
    cJSON_AddNumberToObject(root, "second", 4.56);
    num_direct = cJSON_CreateNumber(9.0);
    cJSON_AddItemToArray(arr, num_direct);

    // step 3: Core operations
    dup_item = cJSON_Duplicate(cJSON_GetObjectItem(root, "second"), 1);
    cJSON_AddItemToArray(arr, dup_item);
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    obj_first = cJSON_GetObjectItem(root, "first");

    // step 4: Validate
    val0 = cJSON_GetNumberValue(arr_item0);
    val_first = cJSON_GetNumberValue(obj_first);
    out = cJSON_PrintUnformatted(root);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(arr_item0 != NULL);
    validation ^= (int)(obj_first != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val0 == 9.0);
    validation ^= (int)(val_first == 1.23);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}