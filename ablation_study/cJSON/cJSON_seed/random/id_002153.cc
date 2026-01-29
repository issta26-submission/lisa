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
//<ID> 2153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *raw = NULL;
    char *printed = NULL;
    cJSON *got_num = NULL;
    cJSON *got_str = NULL;
    cJSON *got_raw = NULL;
    const char *str_val = NULL;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddStringToObject(root, "greeting", "hello");
    raw = cJSON_CreateRaw("\"raw_value\"");
    cJSON_AddItemToObjectCS(root, "raw", raw);

    // step 3: Operate
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_num = cJSON_GetObjectItem(root, "pi");
    got_str = cJSON_GetObjectItem(root, "greeting");
    got_raw = cJSON_GetObjectItem(root, "raw");
    num_val = cJSON_GetNumberValue(got_num);
    str_val = cJSON_GetStringValue(got_str);
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(got_raw != NULL);
    validation ^= (int)(num_val == 3.1415);
    validation ^= (int)(str_val != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}