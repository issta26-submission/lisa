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
//<ID> 2151
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
    cJSON *got_count = NULL;
    char *printed = NULL;
    double val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "count", 123.0);
    raw = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObjectCS(root, "raw_json", raw);

    // step 3: Operate
    printed = cJSON_PrintUnformatted(root);
    got_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    val = cJSON_GetNumberValue(got_count);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_count != NULL);
    validation ^= (int)(val == 123.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}