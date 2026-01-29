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
//<ID> 2152
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
    cJSON *got_num = NULL;
    cJSON *got_str = NULL;
    cJSON *got_raw = NULL;
    char *printed = NULL;
    const char *str_value = NULL;
    cJSON_bool is_num = 0;
    cJSON_bool is_str = 0;
    cJSON_bool has_raw = 0;
    double num_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddStringToObject(root, "hello", "world");
    raw = cJSON_CreateRaw("{\"embedded\":42}");
    cJSON_AddItemToObjectCS(root, "embedded", raw);

    // step 3: Core operations
    printed = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItemCaseSensitive(root, "pi");
    got_str = cJSON_GetObjectItemCaseSensitive(root, "hello");
    got_raw = cJSON_GetObjectItemCaseSensitive(root, "embedded");
    is_num = cJSON_IsNumber(got_num);
    is_str = cJSON_IsString(got_str);
    has_raw = cJSON_IsRaw(got_raw);
    num_value = cJSON_GetNumberValue(got_num);
    str_value = cJSON_GetStringValue(got_str);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(is_str != 0);
    validation ^= (int)(has_raw != 0);
    validation ^= (int)(num_value == 3.1415);
    validation ^= (int)(str_value != NULL && str_value[0] == 'w');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}