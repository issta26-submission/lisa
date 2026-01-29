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
//<ID> 552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"foo\": 123.5 , \"bar\" : true } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddTrueToObject(root, "flag");
    cJSON *str = cJSON_CreateString("Hello cJSON");
    cJSON_AddItemToObject(root, "greeting", str);

    // step 2: Configure
    double foo_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "foo"));
    double set_result = cJSON_SetNumberHelper(num, foo_val);
    (void)set_result;
    cJSON *copied = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_copy", copied);

    // step 3: Operate
    char *snapshot = cJSON_Print(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    memcpy(buffer, snapshot, snaplen);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    char *greeting = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    (void)extracted;
    (void)greeting;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}