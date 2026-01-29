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
//<ID> 1086
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;
    cJSON_bool label_is_null = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create a root object and items
    root = cJSON_CreateObject();
    float samples[3] = { 1.5f, 2.5f, 3.5f };
    arr = cJSON_CreateFloatArray(samples, 3);
    str_item = cJSON_CreateString("sensor-A");

    // step 3: Configure - attach array and string to the root object
    add_ok_arr = cJSON_AddItemToObject(root, "measurements", arr);
    add_ok_label = cJSON_AddItemToObject(root, "id", str_item);

    // step 4: Operate - duplicate the whole tree and fetch items from the duplicate
    dup_root = cJSON_Duplicate(root, (cJSON_bool)1);
    fetched_arr = cJSON_GetObjectItem(dup_root, "measurements");
    fetched_label = cJSON_GetObjectItem(dup_root, "id");
    label_is_null = cJSON_IsNull(fetched_label);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(dup_root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_label) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(!label_is_null);
    (void)validation_score;

    // step 6: Cleanup - delete both original and duplicate trees
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}