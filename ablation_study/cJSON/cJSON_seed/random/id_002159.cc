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
//<ID> 2159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *cs_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_age = NULL;
    cJSON *got_name = NULL;
    char *printed = NULL;
    const char *got_name_str = NULL;
    const char *got_cs_str = NULL;
    cJSON_bool added_cs = 0;
    double age_val = 0.0;
    int validation = 0;

    // step 2: Setup (Initialize object and add typed members)
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "age", 30.0);
    cJSON_AddStringToObject(root, "name", "Alice");
    cs_item = cJSON_CreateString("CaseSensitiveValue");
    added_cs = cJSON_AddItemToObjectCS(root, "CSKey", cs_item);
    raw_item = cJSON_CreateRaw("true");
    cJSON_AddItemToObject(root, "isRawTrue", raw_item);

    // step 3: Operate (serialize and retrieve values)
    printed = cJSON_PrintUnformatted(root);
    got_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(got_age);
    got_name = cJSON_GetObjectItem(root, "name");
    got_name_str = cJSON_GetStringValue(got_name);
    got_cs_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "CSKey"));

    // step 4: Validate (accumulate simple checks into validation)
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_cs != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(age_val == 30.0);
    validation ^= (int)(got_name_str != NULL);
    validation ^= (int)(got_cs_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}