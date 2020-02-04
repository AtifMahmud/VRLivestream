namespace Network {

/// <summary>
/// Handles construction of network objects based on the server type
/// so that it is easier to switch between networt implementations
/// </summary>
public static class NetworkMaster {
    private static string ip_address_ = "";

    static NetworkMaster() {}

    /// <summary>
    /// Sets the ip address to be used by the client
    /// </summary>
    /// <param name="ip_address"></param>
    static public void SetIP(string ip_address) {
        ip_address_ = ip_address;
    }

    static public string GetIP() { return ip_address_; }
}

}  // namespace Network
