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
//<ID> 1159
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(data, s1);
    cJSON_AddItemToArray(data, s2);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "count", n1);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "note", "initial");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *data_ref = cJSON_GetObjectItem(root, "data");
    int size = cJSON_GetArraySize(data_ref);
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "count"));
    double computed_val = (double)size + count_val;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta, "note");
    char *setret = cJSON_SetValuestring(note_ref, "updated");
    (void)setret;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}