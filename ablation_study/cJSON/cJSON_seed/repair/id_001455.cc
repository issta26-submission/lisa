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
//<ID> 1455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *names = cJSON_AddArrayToObject(settings, "names");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "creator", "tester");
    cJSON_AddStringToObject(root, "note", "sequence-check");

    // step 3: Operate and Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *settings_cs = cJSON_GetObjectItemCaseSensitive(root, "settings");
    cJSON *names_cs = cJSON_GetObjectItemCaseSensitive(settings_cs, "names");
    cJSON_bool names_is_array = cJSON_IsArray(names_cs);
    cJSON *first_name_item = cJSON_GetArrayItem(names_cs, 0);
    char *first_name = cJSON_GetStringValue(first_name_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)has_settings);
    scratch[1] = (char)('0' + (int)names_is_array);
    scratch[2] = out_len > 0 ? out[0] : ' ';
    scratch[3] = first_name ? first_name[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}