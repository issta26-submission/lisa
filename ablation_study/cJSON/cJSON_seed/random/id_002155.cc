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
//<ID> 2155
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
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *raw = NULL;
    char *printed = NULL;
    cJSON_bool add_raw_ok = 0;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup (Initialize JSON structure)
    root = cJSON_CreateObject();
    num = cJSON_AddNumberToObject(root, "count", 7.0);
    str = cJSON_AddStringToObject(root, "name", "example");
    raw = cJSON_CreateRaw("{\"flag\":true}");
    add_raw_ok = cJSON_AddItemToObjectCS(root, "raw", raw);

    // step 3: Operate (Serialize and extract values)
    printed = cJSON_PrintUnformatted(root);
    num_val = cJSON_GetNumberValue(num);

    // step 4: Validate (simple consistency checks without branches)
    validation ^= (int)(root != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(str != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(add_raw_ok != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(num_val == 7.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup (free allocated resources)
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}