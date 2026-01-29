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
//<ID> 1362
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
    cJSON *nameItem = NULL;
    cJSON *verItem = NULL;
    cJSON *gotNameItem = NULL;
    cJSON *gotVerItem = NULL;
    const char *nameStr = NULL;
    double verVal = 0.0;
    int validation = 0;
    cJSON_bool addedName = 0;
    cJSON_bool addedVer = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    nameItem = cJSON_CreateString("example");
    verItem = cJSON_CreateNumber(1.23);
    addedName = cJSON_AddItemToObject(root, "name", nameItem);
    addedVer = cJSON_AddItemToObject(root, "version", verItem);

    // step 3: Operate
    gotNameItem = cJSON_GetObjectItem(root, "name");
    gotVerItem = cJSON_GetObjectItem(root, "version");
    nameStr = cJSON_GetStringValue(gotNameItem);
    verVal = cJSON_GetNumberValue(gotVerItem);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(nameItem != NULL);
    validation ^= (int)(verItem != NULL);
    validation ^= (int)(addedName != 0);
    validation ^= (int)(addedVer != 0);
    validation ^= (int)(gotNameItem != NULL);
    validation ^= (int)(gotVerItem != NULL);
    validation ^= (int)(nameStr != NULL);
    validation ^= (int)(nameStr && nameStr[0] == 'e');
    validation ^= (int)(verVal == 1.23);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}