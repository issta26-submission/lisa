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
//<ID> 1149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *data = cJSON_AddArrayToObject(root, "data");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(data, s1);
    cJSON_AddItemToArray(data, s2);
    cJSON_AddFalseToObject(meta, "active");
    cJSON_AddNumberToObject(meta, "scale", 1.5);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "example");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(data);
    cJSON *first = cJSON_GetArrayItem(data, 0);
    char *first_str = cJSON_GetStringValue(first);
    double scale = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(meta, "scale"));
    double computed = (double)count * scale;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *setret = cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(root, "note"), "updated");
    (void)first_str;
    (void)setret;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}