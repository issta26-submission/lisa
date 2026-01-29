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
//<ID> 1861
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
    cJSON *num_first = NULL;
    cJSON *num_second = NULL;
    cJSON *dup_second = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *obj_second = NULL;
    char *out = NULL;
    double val_arr0 = 0.0;
    double val_obj_second = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num_first = cJSON_AddNumberToObject(root, "first", 1.5);
    num_second = cJSON_AddNumberToObject(root, "second", 2.75);

    // step 3: Core operations
    dup_second = cJSON_Duplicate(num_second, 1);
    cJSON_AddItemToArray(arr, dup_second);

    // step 4: Retrieval and validation
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    obj_second = cJSON_GetObjectItem(root, "second");
    val_arr0 = cJSON_GetNumberValue(arr_item0);
    val_obj_second = cJSON_GetNumberValue(obj_second);
    out = cJSON_PrintUnformatted(root);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_first != NULL);
    validation ^= (int)(num_second != NULL);
    validation ^= (int)(dup_second != NULL);
    validation ^= (int)(arr_item0 != NULL);
    validation ^= (int)(obj_second != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_obj_second == 2.75);
    validation ^= (int)(val_arr0 == 2.75);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}