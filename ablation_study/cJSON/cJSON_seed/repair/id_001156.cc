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
//<ID> 1156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, t);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *scale = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(meta, "scale", scale);
    cJSON *note = cJSON_CreateString("note0");
    cJSON_AddItemToObject(meta, "note", note);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(items_ref);
    cJSON *scale_ref = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale_val = cJSON_GetNumberValue(scale_ref);
    double computed_val = (double)count * scale_val;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta, "note");
    char *note_str = cJSON_GetStringValue(note_ref);
    char *setret = cJSON_SetValuestring(note_ref, "updated");
    (void)note_str;
    (void)setret;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}