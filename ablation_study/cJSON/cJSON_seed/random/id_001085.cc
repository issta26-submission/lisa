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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *note_str = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *meta_obj = (cJSON *)0;
    cJSON *meta_label = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_note = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_meta_label = (cJSON *)0;
    cJSON *fetched_null = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool null_check = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    int validation_score = 0;
    float numbers[] = { 1.1f, 2.2f, 3.3f, 4.4f };

    // step 2: Setup - create root, a float array, strings, a null and a nested object
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, 4);
    note_str = cJSON_CreateString("sample note");
    null_item = cJSON_CreateNull();
    meta_obj = cJSON_CreateObject();
    meta_label = cJSON_CreateString("meta_label_value");

    // step 3: Configure - attach items into the root and nested meta object
    add_ok1 = cJSON_AddItemToObject(root, "values", float_arr);
    add_ok2 = cJSON_AddItemToObject(root, "note", note_str);
    add_ok3 = cJSON_AddItemToObject(root, "empty", null_item);
    cJSON_AddItemToObject(root, "meta", meta_obj);
    cJSON_AddItemToObject(meta_obj, "label", meta_label);

    // step 4: Operate - duplicate the whole root and fetch various items
    dup_root = cJSON_Duplicate(root, (cJSON_bool)1);
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_note = cJSON_GetObjectItem(root, "note");
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    fetched_meta_label = cJSON_GetObjectItem(fetched_meta, "label");
    fetched_null = cJSON_GetObjectItem(root, "empty");
    null_check = cJSON_IsNull(fetched_null);

    // step 5: Validate - serialize duplicate to a string and compute a small score
    printed = cJSON_PrintUnformatted(dup_root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(dup_root != (cJSON *)0) + (int)(fetched_values != (cJSON *)0) + (int)(fetched_note != (cJSON *)0) + (int)(fetched_meta_label != (cJSON *)0) + (int)(null_check);

    // step 6: Cleanup - delete both original and duplicate trees
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}