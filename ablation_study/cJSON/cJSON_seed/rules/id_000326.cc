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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"pi\":3.1415, \"msg\":\"hi\"}";
    size_t json_len = 25;
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_num = cJSON_CreateNumber(2.7182818);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *pi_item = NULL;
    double pi_val = 0.0;
    cJSON *added_pi = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created_value", created_num);
    cJSON_AddNumberToObject(root, "direct_added", 7.0);
    pi_item = cJSON_GetObjectItem(parsed, "pi");
    pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_AddNumberToObject(root, "pi_from_parsed", pi_val);
    added_pi = cJSON_GetObjectItem(root, "pi_from_parsed");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)pi_val);
    buffer[1] = added_pi->string[0];
    buffer[2] = printed ? printed[0] : (char)0;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}