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
//<ID> 2156
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
    cJSON *sitem = NULL;
    cJSON *nitem = NULL;
    cJSON *got_s = NULL;
    cJSON *got_n = NULL;
    char *printed = NULL;
    const char *got_str = NULL;
    double got_num = 0.0;
    cJSON_bool is_str = 0;
    cJSON_bool is_num = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"inlined\":true}");
    cJSON_AddItemToObjectCS(root, "raw", raw);
    sitem = cJSON_AddStringToObject(root, "name", "cJSON");
    nitem = cJSON_AddNumberToObject(root, "version", 123.0);

    // step 3: Operate
    printed = cJSON_PrintUnformatted(root);
    got_s = cJSON_GetObjectItemCaseSensitive(root, "name");
    got_n = cJSON_GetObjectItem(root, "version");
    got_str = cJSON_GetStringValue(got_s);
    got_num = cJSON_GetNumberValue(got_n);
    is_str = cJSON_IsString(got_s);
    is_num = cJSON_IsNumber(got_n);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(sitem != NULL);
    validation ^= (int)(nitem != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(is_str != 0);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(got_num == 123.0);
    buf[0] = (char)(validation + '0');
    buf[1] = (char)((got_str != NULL) ? got_str[0] : '0');
    buf[2] = (char)((printed != NULL) ? printed[0] : '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}