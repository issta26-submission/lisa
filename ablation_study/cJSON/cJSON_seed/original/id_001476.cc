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
//<ID> 1476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"person\":{\"age\":30,\"scores\":[5,10]},\"meta\":{\"keep\":\"yes\",\"remove\":\"bye\"}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *age = cJSON_GetObjectItem(person, "age");
    cJSON *scores = cJSON_GetObjectItem(person, "scores");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(scores, newnum);
    cJSON *meta_ref = cJSON_CreateObjectReference(meta);
    cJSON_AddItemReferenceToArray(scores, meta_ref);
    cJSON *detached = cJSON_DetachItemFromObject(meta, "remove");
    cJSON_DeleteItemFromObject(meta, "keep");
    cJSON_Delete(detached);

    // step 3: Operate & Validate
    cJSON_bool age_is_number = cJSON_IsNumber(age);
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    double age_val = cJSON_GetNumberValue(age);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    (void)age_is_number;
    (void)has_person;
    (void)age_val;

    // API sequence test completed successfully
    return 66;
}