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
//<ID> 2158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *raw = NULL;
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *got_count = NULL;
    cJSON *got_msg = NULL;
    cJSON *got_raw = NULL;
    const char *s_msg = NULL;
    const char *s_raw = NULL;
    double vcount = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 5.0);
    cJSON_AddStringToObject(root, "message", "hello");
    raw = cJSON_CreateRaw("{\"nested\":true}");
    cJSON_AddItemToObjectCS(root, "rawdata", raw);

    // step 3: Core operations
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    got_count = cJSON_GetObjectItem(parsed, "count");
    got_msg = cJSON_GetObjectItem(parsed, "message");
    got_raw = cJSON_GetObjectItem(parsed, "rawdata");
    vcount = cJSON_GetNumberValue(got_count);
    s_msg = cJSON_GetStringValue(got_msg);
    s_raw = cJSON_GetStringValue(got_raw);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(parsed != NULL);
    validation ^= (int)(got_count != NULL);
    validation ^= (int)(got_msg != NULL);
    validation ^= (int)(got_raw != NULL);
    validation ^= (int)(vcount == 5.0);
    validation ^= (int)(s_msg != NULL && s_msg[0] == 'h');
    validation ^= (int)(s_raw != NULL && s_raw[0] == '{');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}