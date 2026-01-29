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
//<ID> 1157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *colors = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("red");
    cJSON *s2 = cJSON_CreateString("green");
    cJSON *s3 = cJSON_CreateString("blue");
    cJSON_AddItemToArray(colors, s1);
    cJSON_AddItemToArray(colors, s2);
    cJSON_AddItemToArray(colors, s3);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "note", note);
    cJSON *ver = cJSON_CreateNumber(1.25);
    cJSON_AddItemToObject(meta, "version", ver);

    // step 2: Configure
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *colors_ref = cJSON_GetObjectItem(root, "colors");
    int count = cJSON_GetArraySize(colors_ref);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *ver_ref = cJSON_GetObjectItemCaseSensitive(meta_ref, "version");
    double ver_val = cJSON_GetNumberValue(ver_ref);
    double computed_val = (double)count * ver_val;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *setret = cJSON_SetValuestring(note, "updated");
    cJSON *note_ref = cJSON_GetObjectItemCaseSensitive(meta_ref, "note");
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