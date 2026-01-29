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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(data, s1);
    cJSON_AddItemToArray(data, s2);
    cJSON_AddItemToArray(data, n3);
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 2.5);
    cJSON_AddFalseToObject(meta, "active");
    cJSON_AddStringToObject(meta, "note", "initial");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *data_ref = cJSON_GetObjectItem(root, "data");
    int count = cJSON_GetArraySize(data_ref);
    cJSON *scale_ref = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale_val = cJSON_GetNumberValue(scale_ref);
    double computed = (double)count * scale_val;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta, "note");
    char *note_val = cJSON_GetStringValue(note_ref);
    char *setret = cJSON_SetValuestring(note_ref, "updated");
    (void)note_val;
    (void)setret;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}