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
//<ID> 2150
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
    char *out = NULL;
    const char *raw_value = NULL;
    int validation = 0;

    // step 2: Setup / Initialize JSON structure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", 100.0);
    cJSON_AddStringToObject(root, "name", "tester");
    raw = cJSON_CreateRaw("{\"nested\":123}");
    cJSON_AddItemToObjectCS(root, "raw", raw);

    // step 3: Operate (serialize and inspect)
    out = cJSON_PrintUnformatted(root);
    raw_value = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(root, "raw"));
    buf[0] = (char)('0'); // initialize a visible location

    // step 4: Validate results (no branches)
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(raw_value != NULL);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(out);

    // API sequence test completed successfully
    return 66;
}