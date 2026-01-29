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
//<ID> 1331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"pi\": 3.14159, \"msg\": \"hello\"}";
    cJSON *root = NULL;
    cJSON *pi_item = NULL;
    cJSON *msg_item = NULL;
    double pi_value = 0.0;
    cJSON *meta = NULL;
    cJSON *double_pi = NULL;
    cJSON *note_str = NULL;
    char *printed = NULL;
    cJSON *detached_meta = NULL;
    double extracted_double = 0.0;
    const char *extracted_msg = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    pi_item = cJSON_GetObjectItem(root, "pi");
    msg_item = cJSON_GetObjectItem(root, "msg");
    pi_value = cJSON_GetNumberValue(pi_item);
    meta = cJSON_CreateObject();
    double_pi = cJSON_CreateNumber(pi_value * 2.0);
    cJSON_AddItemToObject(meta, "double_pi", double_pi);
    note_str = cJSON_CreateString("created_string");
    cJSON_AddItemToObject(meta, "note", note_str);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    detached_meta = cJSON_DetachItemFromObject(root, "meta");
    extracted_double = cJSON_GetNumberValue(cJSON_GetObjectItem(detached_meta, "double_pi"));
    extracted_msg = cJSON_GetStringValue(msg_item);
    (void)extracted_double;
    (void)extracted_msg;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_meta);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}