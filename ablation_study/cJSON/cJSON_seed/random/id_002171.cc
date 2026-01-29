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
//<ID> 2171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *added_direct = NULL;
    char *printed = NULL;
    cJSON_bool arr_check = 0;
    int validation = 0;

    // step 2: Setup (create root object and members)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    added_direct = cJSON_AddBoolToObject(root, "flag_direct", 1);
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_true", true_item);
    cJSON_AddItemToObject(root, "flag_false", false_item);

    // step 3: Operate (serialize and check types)
    printed = cJSON_PrintUnformatted(root);
    arr_check = cJSON_IsArray(arr);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(added_direct != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(arr_check != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}