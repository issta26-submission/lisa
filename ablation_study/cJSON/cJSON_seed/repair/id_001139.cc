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
//<ID> 1139
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
    cJSON *t_created = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, t_created);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "flag");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("note0");
    cJSON_AddItemToObject(meta, "note", note);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    double computed = (double)size + 2.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *setret = cJSON_SetValuestring(note, "updated");
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta, "note");
    char *note_val = cJSON_GetStringValue(note_ref);
    (void)setret;
    (void)note_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}