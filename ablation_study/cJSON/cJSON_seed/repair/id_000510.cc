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
//<ID> 510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"name\":\"tester\",\"enabled\":false,\"count\":2}";
    const char *parse_end = NULL;
    size_t buffer_length = (sizeof("{\"name\":\"tester\",\"enabled\":false,\"count\":2}") - 1);
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, buffer_length, &parse_end, 1);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *generated = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "generated", generated);
    double multiplied = count_value * 10.0;
    cJSON *multiplied_num = cJSON_CreateNumber(multiplied);
    cJSON_AddItemToObject(generated, "multiplied", multiplied_num);
    cJSON *cfg = cJSON_AddBoolToObject(root, "configured", 1);
    char temp_buf[64];
    memset(temp_buf, 0, sizeof(temp_buf));
    cJSON *note = cJSON_CreateString("configured by test");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_bool has_generated = cJSON_HasObjectItem(root, "generated");
    (void)has_generated;
    cJSON_bool has_configured = cJSON_HasObjectItem(root, "configured");
    (void)has_configured;
    (void)cfg;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}