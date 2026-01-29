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
//<ID> 1036
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"alpha\",\"items\":[{\"id\":1,\"label\":\"one\"},{\"id\":2,\"label\":\"two\"}],\"duplicate_label\":\"one\"}";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item0_label = (cJSON *)0;
    cJSON *dup_label = (cJSON *)0;
    char *label_str0 = (char *)0;
    char *label_str_dup = (char *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool equal = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into root
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain array and label items
    arr = cJSON_GetObjectItem(root, "items");
    item0 = cJSON_GetArrayItem(arr, 0);
    item0_label = cJSON_GetObjectItem(item0, "label");
    dup_label = cJSON_GetObjectItem(root, "duplicate_label");

    // step 4: Operate - extract string values, get array size, compare and print unformatted
    label_str0 = cJSON_GetStringValue(item0_label);
    label_str_dup = cJSON_GetStringValue(dup_label);
    arr_size = cJSON_GetArraySize(arr);
    equal = cJSON_Compare(item0_label, dup_label, (cJSON_bool)1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a small validation score and free printed buffer
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(item0 != (cJSON *)0) + (int)(item0_label != (cJSON *)0) + (int)(dup_label != (cJSON *)0) + (int)(label_str0 != (char *)0) + (int)(label_str_dup != (char *)0) + (int)(arr_size == 2) + (int)(equal) + (int)(printed != (char *)0);
    (void)validation_score;
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}