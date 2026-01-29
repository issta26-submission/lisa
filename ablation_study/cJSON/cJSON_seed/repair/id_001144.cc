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
//<ID> 1144
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
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *s1 = cJSON_CreateString("element");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddNumberToObject(meta, "scale", 2.5);
    cJSON_AddStringToObject(meta, "info", "initial");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double computed_val = first_val * scale + (double)arr_size;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *info_ref = cJSON_GetObjectItemCaseSensitive(meta, "info");
    char *setret = cJSON_SetValuestring(info_ref, "updated");
    char *info_val = cJSON_GetStringValue(info_ref);
    (void)setret;
    (void)info_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}