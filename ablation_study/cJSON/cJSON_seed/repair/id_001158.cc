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
//<ID> 1158
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("noteX");
    cJSON_AddItemToObject(meta, "note", note);
    cJSON *mult = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(meta, "mult", mult);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta_ref, "note");
    const char *note_str = cJSON_GetStringValue(note_ref);
    double multiplier = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(meta_ref, "mult"));
    double computed_value = (double)count * multiplier + (double)note_str[0];
    cJSON *computed_item = cJSON_CreateNumber(computed_value);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}