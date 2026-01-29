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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    char *dyn_key = (char*)cJSON_malloc(8);
    memset(dyn_key, 0, 8);
    memcpy(dyn_key, "greet", 6);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, dyn_key, greeting);
    cJSON *lang = cJSON_CreateString("C++");
    cJSON_AddItemToObject(root, "lang", lang);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "lib", ver);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool has_greet = cJSON_HasObjectItem(dup, dyn_key);
    cJSON_bool has_lang = cJSON_HasObjectItem(dup, "lang");
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(flags, "has_greet", cJSON_CreateBool(has_greet));
    cJSON_AddItemToObject(flags, "has_lang", cJSON_CreateBool(has_lang));
    cJSON_AddItemToObject(dup, "flags", flags);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_free(dyn_key);

    // API sequence test completed successfully
    return 66;
}