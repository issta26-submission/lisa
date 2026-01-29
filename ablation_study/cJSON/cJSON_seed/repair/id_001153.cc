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
//<ID> 1153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *scale_num = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(meta, "scale", scale_num);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "note", note);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    int size = cJSON_GetArraySize(items_ref);
    cJSON *scale_ref = cJSON_GetObjectItem(meta, "scale");
    double scale_value = cJSON_GetNumberValue(scale_ref);
    double computed_val = (double)size * scale_value;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    cJSON *note_ref_case = cJSON_GetObjectItemCaseSensitive(root, "note");
    char *note_before = cJSON_GetStringValue(note_ref_case);
    char *setret = cJSON_SetValuestring(note_ref_case, "updated");
    (void)note_before;
    (void)setret;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}