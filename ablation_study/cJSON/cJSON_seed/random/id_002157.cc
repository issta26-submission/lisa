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
//<ID> 2157
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
    cJSON *raw = NULL;
    cJSON *got_raw = NULL;
    cJSON *got_num = NULL;
    cJSON *got_str = NULL;
    cJSON_bool is_num = 0;
    double vnum = 0.0;
    char *printed = NULL;
    int validation = 0;

    // step 2: Setup (Initialize object and add items)
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddStringToObject(root, "name", "example_name");
    raw = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObjectCS(root, "raw_cs", raw);

    // step 3: Operate (serialize and retrieve items)
    printed = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItem(root, "version");
    got_str = cJSON_GetObjectItem(root, "name");
    got_raw = cJSON_GetObjectItem(root, "raw_cs");
    is_num = cJSON_IsNumber(got_num);
    vnum = cJSON_GetNumberValue(got_num);

    // step 4: Validate (accumulate simple validations)
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(vnum == 1.0);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(cJSON_GetStringValue(got_str) != NULL);
    validation ^= (int)(got_raw != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}