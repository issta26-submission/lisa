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
//<ID> 1147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *note = cJSON_AddStringToObject(meta, "note", "original");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first = cJSON_GetArrayItem(items_ref, 0);
    double first_val = cJSON_GetNumberValue(first);
    double computed_val = first_val * 2.0 + 1.0;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta, "note");
    char *note_str = cJSON_GetStringValue(note_ref);
    cJSON *note_copy = cJSON_CreateString(note_str);
    cJSON_AddItemToObject(root, "note_copy", note_copy);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}